#include <unordered_set>
#include<bits/stdc++.h>
#include<iostream>
#include<string>
#include<cstring>
#include<typeinfo>
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;
using namespace std;



class CheckMallocVisitor : public RecursiveASTVisitor<CheckMallocVisitor> {
private:

	// maintains a context to the AST
	ASTContext *context;
	CompilerInstance& instance;
	
	// Map to track all the variables, variable name : global counter
	unordered_map<string, int> varCountMap;

	// Map to track the malloc variables, variable name : boolean (if malloc or not)
	unordered_map<string, int> mallocVars;

	// Map to track the start and end line of the if blocks
	unordered_map<string, pair<int, int>> mallocIfs;

	/**
	 * true -> on seeing it as malloc 
	 * false -> on verifying return from malloc  
	 * 
	 * 
	 * or 
	 * 0 -> on seeing malloc
	 * 1 -> on having if condition
	 * 2 -> on checking that it is within the scope
	 */

	// To get ifs and check for null return from malloc

	DiagnosticsEngine& d;

	// global counter
	unsigned int count = 0;
	unsigned int noCheck;
	unsigned int notInIf;

	void getCount(){
		count += 1;
		//cout << "count: " << count << endl;
	}

	// Debug functions
	void printVars(){
		for(auto x : varCountMap){
			cout << x.first << " " << x.second << "\n";
		}
	}

	void printAllIfs(){
		for(auto x : mallocIfs){
			cout << "At location " << x.first << " s " << x.second.first << " e " << x.second.second << "\n";
		}
	}

	void printMallocVars(){
		cout << "\n MALLOC VARS " << "\n";
		for(auto x : mallocVars){
			cout << x.first << " " << x.second << "\n";
		}
	}

	std::string removePointer(std::string str)
	{
		str.erase(remove(str.begin(), str.end(), '*'), str.end());
		return str;
	}

	std::string convertExpressionToString(Expr *E) {
		SourceManager &SM = context->getSourceManager();
		clang::LangOptions lopt;
		SourceLocation startLoc = E->getBeginLoc();
		SourceLocation _endLoc = E->getEndLoc();
		SourceLocation endLoc = clang::Lexer::getLocForEndOfToken(_endLoc, 0, SM, lopt);
		
		// for declaration
		string exp = std::string(SM.getCharacterData(startLoc), SM.getCharacterData(endLoc) - SM.getCharacterData(startLoc)); 
		//cout << "EXPRESSION IS " << exp << "\n";
		exp = removePointer(exp);

		std::string mall = "malloc";
		if (exp.find("malloc") != std::string::npos) {
			for(auto x : varCountMap){
				//cout << "Var is  : " << x.first << "count is  : " << x.second << "\n";
				if(x.second == count - 1){ 
					//cout << "FOUND HERE " << x.first << "\n";
					mallocVars[x.first] = 0;
				}
			}
		}
		//printMallocVars();
		return exp;
	}

	// Omit the declarations that are present in the inbuilt header
	bool isInHeaderDecl(Decl *decl) {
		auto loc = decl->getLocation();
		auto floc = context->getFullLoc(loc);
		if (floc.isInSystemHeader()) return true;
		auto entry = floc.getFileEntry()->getName();
		if (entry.endswith(".h") || entry.endswith(".hpp")) {
			return true;
		}
		return false;
	}

	// Omit the statements that are present in the inbuilt header
    bool isInHeaderStmt(Stmt* stmt) {
		auto loc = stmt->getBeginLoc();
		auto floc = context->getFullLoc(loc);
		if (floc.isInSystemHeader()) return true;
		auto entry = floc.getFileEntry()->getName();
		if (entry.endswith(".h") || entry.endswith(".hpp")) {
			return true;
		}
		return false;
	}

	string getVarName(string var){
		int index = 0;
		string allow = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
		while(index < var.length() && allow.find(var[index]) != string::npos){
			index += 1;
		}
		string h =  var.substr(0, index);
		return var.substr(0, index);
	}

	public:
	explicit CheckMallocVisitor(ASTContext *context, CompilerInstance& instance): 
			context(context), instance(instance), d(instance.getDiagnostics()) {
	
		noCheck = d.getCustomDiagID(DiagnosticsEngine::Warning,
				"Pointer not checked for null return from malloc : '%0'");
		
		notInIf = d.getCustomDiagID(DiagnosticsEngine::Warning,
				"Pointer assignment not done in if block : '%0'");
	}


	/**
	 * In a declaration statement, get the variable name and assign it to the 
	 * global counter
	 */
	virtual bool VisitDeclStmt(DeclStmt *s) {
		if (isInHeaderStmt(s)) {
			return true;
		}
		getCount();
		for (auto decl : s->decls()) {
			if (auto var = dyn_cast<VarDecl>(decl)) {
				varCountMap[var->getNameAsString()] = count;
			}
		}
		return true;
	}

	/**
	 * For a pointer or cast expression, convert the expression to string
	 */
	virtual bool VisitCStyleCastExpr(CStyleCastExpr *E) {
		if(isInHeaderStmt(E)) {
			return true;
		}
		getCount();
		string expr = convertExpressionToString(E);
		return true;
	}
    
	/**
	 * Apart from an assignment, any other statement with =, 
	 * is considered as a binary operator
	 */
    virtual bool VisitBinaryOperator(BinaryOperator *s) {
		if(isInHeaderStmt(s)){
            return true;
        }
		getCount();
        //cout << "-------- IN THE BINARY OPERATOR --------" << "\n";
		string LHS = getVarName(removePointer(convertExpressionToString(s->getLHS())));
		//cout << "LHS is " << LHS << "\n";

		string RHS = removePointer(convertExpressionToString(s->getRHS()));

		//printMallocVars();

		if(s->isAssignmentOp()){
			auto loc = context->getFullLoc(s->getExprLoc());
			string s1 = s->getExprLoc().printToString(context->getSourceManager());
			int assignmentPos = getCol(s1);
			// if malloc is present on the RHS, it is the case of an assignment to malloc
			if (RHS.find("malloc") != std::string::npos) {
				mallocVars[LHS] = 0; // indicating it is a malloc var, and is found
				return true;
			}

			//if malloc is not on the RHS, it means it is an assignment to a value 
			// check if it is done here, the checking, i.e this is where the assignment and checking happens
			if(mallocVars.find(LHS) != mallocVars.end()){ // && ifVars[LHS].first <= s1 <= ifVars[LHS].second)
				 //There is no if check -> being used
				//cout << "DEBUG	 in malloc vars " << LHS << " " << mallocVars[LHS] << "\n"; 
				if(mallocVars[LHS] == 0){
					// indicating that it is a malloc var, and no check has been performed
					d.Report(loc, noCheck) << LHS; 
					return true;
				}
				else if (mallocVars[LHS] == 1){
					// if there is an if block associated to the variable, check if the assignment is within the if block
					if(mallocIfs.find(LHS) != mallocIfs.end()){
						if(mallocIfs[LHS].first <= assignmentPos && assignmentPos <= mallocIfs[LHS].second){
							mallocVars[LHS] = 2; // check done in the if block, no error
							return true;
						}
						else if(mallocIfs[LHS].first > assignmentPos || assignmentPos >  mallocIfs[LHS].second){
							d.Report(loc, notInIf) << LHS; // there is an if, but the malloc is outside the if
							return true;
						}
					}
				}
					
				}
			} 
		
		return true;
	}

	/**
	 * For every if statement, the expression is checked, if it is an inequality check 
	 * for malloc, then the start and end position of the if block is recorded
	 */
    virtual bool VisitIfStmt(IfStmt *s) {
        if(isInHeaderStmt(s)){
            return true;
        }
		getCount();
		pair<int, int> pos;
		string ifstmt = convertExpressionToString(s->getCond());
		string start = s->getBeginLoc().printToString(context->getSourceManager());
		string end = s->getEndLoc().printToString(context->getSourceManager());
		int startPos = getCol(start);
		int endPos = getCol(end);
		for(auto x : s->children()){	
			if(string(x->getStmtClassName()) == "BinaryOperator"){
				BinaryOperator *b = dyn_cast<BinaryOperator>(x);
				string LHS = getVarName(removePointer(convertExpressionToString(b->getLHS())));
				string RHS = removePointer(convertExpressionToString(b->getRHS()));
				if(b->isEqualityOp() && mallocVars.find(LHS) != mallocVars.end() && RHS == "nullptr"){
					// indicating that an if block performing check against null ptr is present, set value to 1 in mallocVars
					mallocVars[LHS] = 1;
					pos = make_pair(startPos, endPos); 

					// record the start and end location of the if block
					mallocIfs[LHS] = pos;
				}
			}
		}
		return true;
	}

	// split the location at : and return the column number
	int getCol(string loc){
		char* str = new char[loc.length() + 1];
		std::copy(loc.begin(), loc.end(), str);
		char *token = strtok(str, ":");
		vector<string>position;
		while (token != NULL){
			position.push_back(string(token));
			token = strtok(NULL, ":");
		}
		return stoi(position[1]);
	}
};

class CheckMallocConsumer : public ASTConsumer {
	CompilerInstance& instance;
	CheckMallocVisitor visitor;

public:
	CheckMallocConsumer(CompilerInstance& instance)
			: instance(instance), visitor(&instance.getASTContext(), instance) {}

	virtual void HandleTranslationUnit(ASTContext &context) override {
		visitor.TraverseDecl(context.getTranslationUnitDecl());
	}
};

class CheckMallocAction : public PluginASTAction {
protected:
	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& instance, llvm::StringRef) override {
		return std::make_unique<CheckMallocConsumer>(instance);
	}

	virtual bool ParseArgs(const CompilerInstance&, const std::vector<std::string>&) override {
		return true;
	}

	virtual PluginASTAction::ActionType getActionType() override {
  		return PluginASTAction::AddAfterMainAction;
	}
};

static FrontendPluginRegistry::Add<CheckMallocAction> CheckMalloc("CheckMalloc", "Warn against unchecked usage of return from malloc");

//set it to 2  or fine 
// if within the range and previous is 1, then fine
// if previous is 0 then no if, raise not checked error
//if not within range raise not assigned in if block 
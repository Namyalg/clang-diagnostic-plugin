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
	
	// Map to track the malloc variables, variable name : 0, 1
	// 0 -> found a malloc var
	// 1 -> found a null check for the malloc var
	unordered_map<string, int> mallocVars;

	// Track the start and end line of the if blocks for malloc vars
	unordered_map<string, pair<int, int>> mallocIfs;

	DiagnosticsEngine& d;

	// global counter
	unsigned int count = 0;
	unsigned int noCheck;
	unsigned int notInIf;

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
		exp = removePointer(exp);
		return exp;
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
	 * In a variable declaration statement, get the variable name and assign it to the 
	 * global counter
	 */
	virtual bool VisitVarDecl(VarDecl *s) {
		if (isInHeaderDecl(s)) {
			return true;
		}
		// if no initializer, returns 0
		if (s->getInit()) {
			Expr *E = s->getInit();
			string exp = convertExpressionToString(E);
			if (string(s->getInit()->getStmtClassName()) == "CStyleCastExpr" &&
				exp.find("malloc") != std::string::npos) {
				mallocVars[s->getNameAsString()] = 0;
			}
		}
		return true;
	}
    
	/**
	 * Apart from an var decl, any other statement with =, 
	 * is considered as a binary operator
	 */
    virtual bool VisitBinaryOperator(BinaryOperator *s) {
		if(isInHeaderStmt(s)){
            return true;
        }

		string LHS = getVarName(removePointer(convertExpressionToString(s->getLHS())));
		string RHS = removePointer(convertExpressionToString(s->getRHS()));
		
		if(s->isAssignmentOp()){
			auto loc = context->getFullLoc(s->getExprLoc());
			string s1 = s->getExprLoc().printToString(context->getSourceManager());
			int assignmentPos = getCol(s1);
			// if malloc is present on the RHS, it is the case of an assignment to malloc
			if (string(s->getRHS()->getStmtClassName()) == "CStyleCastExpr" &&
				RHS.find("malloc") != std::string::npos) {
				mallocVars[LHS] = 0; // indicating it is a malloc var, and is found
				return true;
			}

			//if malloc is not on the RHS, it means it is an assignment to a value 
			// check if a malloc var is assigned a value, i.e this is where the assignment and checking happens
			if(mallocVars.find(LHS) != mallocVars.end()){ // && ifVars[LHS].first <= s1 <= ifVars[LHS].second)
				 //There is no if check -> being used
				if(mallocVars[LHS] == 0){
					// indicating that it is a malloc var, and no check has been performed
					d.Report(loc, noCheck) << LHS; 
					return true;
				}
				else if (mallocVars[LHS] == 1){
					// if there is an if block associated to the variable, 
					// check if the assignment is within the if block
					if(mallocIfs[LHS].first > assignmentPos || assignmentPos >  mallocIfs[LHS].second){
							d.Report(loc, notInIf) << LHS; // there is an if, but the malloc is outside the if
							return true;
						}
						return true;
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

		pair<int, int> pos;
		string ifstmt = convertExpressionToString(s->getCond());
		// start and end locations of the if block
		string start = s->getBeginLoc().printToString(context->getSourceManager());
		string end = s->getEndLoc().printToString(context->getSourceManager());
		int startPos = getCol(start);
		int endPos = getCol(end);
		for(auto x : s->children()){	
			if(string(x->getStmtClassName()) == "BinaryOperator"){
				BinaryOperator *b = dyn_cast<BinaryOperator>(x);
				string LHS = getVarName(removePointer(convertExpressionToString(b->getLHS())));
				string RHS = removePointer(convertExpressionToString(b->getRHS()));
				// there is an if check for malloc for the malloc variable
			
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

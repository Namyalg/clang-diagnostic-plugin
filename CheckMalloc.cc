#include <unordered_set>
#include<bits/stdc++.h>
#include<iostream>
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
	ASTContext *context;
	CompilerInstance& instance;
	unordered_map<string, int> varCountMap;
	unordered_map<string, bool> mallocVars;
	DiagnosticsEngine& d;
	unsigned int count = 0;
	unsigned int warningID;

	void getCount(){
		count += 1;
	}

	void printVars(){
		for(auto x : varCountMap){
			cout << x.first << " " << x.second << "\n";
		}
	}

	std::string removePointer(std::string str)
	{
		str.erase(remove(str.begin(), str.end(), '*'), str.end());
		return str;
	}

	void printMallocVars(){
		//cout << "\n MALLOC VARS " << "\n";
		for(auto x : mallocVars){
			cout << x.first << " " << x.second << "\n";
		}
	}

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

	// void checkName(std::string name, Decl *decl) {
	// 	if (name.at(0) == '_') {
	// 		auto loc = context->getFullLoc(decl->getLocation());
	// 		d.Report(loc, warningID) << name;
	// 	}
	// }

public:
	explicit CheckMallocVisitor(ASTContext *context, CompilerInstance& instance): 
			context(context), instance(instance), d(instance.getDiagnostics()) {
		warningID = d.getCustomDiagID(DiagnosticsEngine::Warning,
				"Pointer not checked for null return from malloc : '%0'");
	}


	virtual bool VisitDeclStmt(DeclStmt *s) {
		if (isInHeaderStmt(s)) {
			return true;
		}
		getCount();
		for (auto decl : s->decls()) {
			if (auto var = dyn_cast<VarDecl>(decl)) {
				//llvm :: errs() << "var : " << var->getNameAsString() << "\n";
				varCountMap[var->getNameAsString()] = count;
			}
		}
		// cout << "ALL VARS GOT " << "\n";
		// printVars();
		return true;
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
			//cout << "found malloc in exp " << exp << "\n";
			//cout << "The current value of count is " << count << "\n";
			for(auto x : varCountMap){
				//cout << "Var is  : " << x.first << "count is  : " << x.second << "\n";
				if(x.second == count - 1){ 
					//cout << "FOUND HERE " << x.first << "\n";
					mallocVars[x.first] = false;
				}
			}
		}
		//printMallocVars();
		return exp;
	}

	virtual bool VisitCStyleCastExpr(CStyleCastExpr *E) {
		if(isInHeaderStmt(E)) {
			return true;
		}
		getCount();
		//cout << "CSTYLE CAST EXPR " << count << "\n";
		string RHS = convertExpressionToString(E);
		//cout << RHS << '\n';
		return true;
	}
        
    virtual bool VisitBinaryOperator(BinaryOperator *s) {
		if(isInHeaderStmt(s)){
            return true;
        }
		getCount();
        //cout << "-------- IN THE BINARY OPERATOR --------" << "\n";
		string LHS = removePointer(convertExpressionToString(s->getLHS()));
		string RHS = removePointer(convertExpressionToString(s->getRHS()));


		//cout << "LHS of binary op " << LHS << "\n";
		//cout << "RHS of binary op " << RHS << "\n";
		if(s->isEqualityOp()){	
			if(RHS =="0" && mallocVars.find(LHS) != mallocVars.end()){ // is present
				mallocVars[LHS] = true;
			}
		}

		if(s->isAssignmentOp()){
			//cout << "IN ASSSIGNMENT OP" << "\n";
			
			// if ( != std::string::npos) {
			if (RHS.find("malloc") != std::string::npos) {
				mallocVars[LHS] = false;
				
				return true;
			}
			 
			if(mallocVars.find(LHS) != mallocVars.end()){
				
				//cout << "MALLOC VAR FOUND " << "\n";
				if(!mallocVars[LHS]){ // There is no if check -> being used 
						auto loc = context->getFullLoc(s->getBeginLoc());
						d.Report(loc, warningID) << LHS;
				}
			} 
		}

		//printMallocVars();
		return true;
	}

    virtual bool VisitIfStmt(IfStmt *s) {
        if(isInHeaderStmt(s)){
            return true;
        }
		getCount();
		//cout << "***************** IN THE IF STMT ***********************" << "\n";
		string ifstmt = convertExpressionToString(s->getCond());
		//cout <<  << "\n";
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

static FrontendPluginRegistry::Add<CheckMallocAction> CheckMalloc("CheckMalloc", "Warn against underscores in function and variable names");

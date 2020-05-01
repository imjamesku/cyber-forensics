
#include "llvm/Pass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>
#include <set>
using namespace llvm;
using namespace std;

#define DBG errs() << "[DBG] " << __FILE__ << ":" << __LINE__ << "\n";
//#define NULL 0

namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct MyLLVMPass : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    MyLLVMPass() : ModulePass(ID) {}
	
	
	vector<Value*> m_vtGV;
	vector<string> m_vtTargetFuncName;
	
	
	

	
	void GetGlobalVariableList(Module& M)
	{
		for(auto G=M.getGlobalList().begin(); G!=M.getGlobalList().end(); G++){
			GlobalVariable &GV = *G;
			if(GV.isConstant()){
				//errs() << GV.getName() << "\n";
				//errs() << GV << "\n";
				m_vtGV.push_back( G );
			}
		}
	}
	
	bool IsGlobalVariable(Value* pV)
	{
		for( unsigned int i = 0; i < m_vtGV.size(); i++ ) {
			Value* p = m_vtGV[i];
			
			//errs() << p->getValueID() << "vs." << pV->getValueID() << "\n";
			if( p->getValueID() == pV->getValueID() ) {
				return true;
			}
		}
		return false;
	}
	
	string GetFunctionCallTarget(Instruction* I)
	{
		CallSite cs(I);
		if( cs.getInstruction() ) {
			Value* target = cs.getCalledValue()->stripPointerCasts();
			Function* f = dyn_cast<Function>(target);
			if( f ) {
				return f->getName().str();
			}
		}
		return string("");
	}
	
	string mapFind(map<string, string>& m, string key)
	{
		map<string,string>::iterator i;
		
		i = m.find( key );
		if( i != m.end() ) { // found
			return i->second;
		}
		
		return string("");		
	}
	
	map<string, set<string>*> m_mapAlias;
	void NewAlias(string dest, string src)
	{
		set<string>* pset = m_mapAlias[dest];
		
		if( pset == NULL ) {
			pset = new set<string>;
			m_mapAlias[dest] = pset;
		}
		
		pset->insert( src );
		errs() << "[NA]: " << dest << " : " << src << "\n";
	}
	
	set<string>* GetAlias(string dest)
	{
		return m_mapAlias[dest];
	}
	
	void printAlias(string dest)
	{
		set<string>* pset = m_mapAlias[dest];
		
		errs() << dest << "\n";
		DBG;
		if( pset ) {
			for( set<string>::iterator it = pset->begin(); it != pset->end(); it++ ) {
				errs() << "printAlias: " << *it << "\n";
			}
		}
		DBG;
	}
	
	vector<string>* GetAllAliasVars( Value* v ) 
	{
		vector<string>* pvt = new vector<string>;
		if( isa<LoadInst>(v) ) {
			LoadInst* LI = cast<LoadInst>(v);
			
			//errs()<< *LI << "---------\n";
			//errs()<< LI->getNumOperands() << "---------\n";
			
			Value* vLoad = LI->getOperand(0)->stripPointerCasts();
			
			set<string>* p = GetAlias( vLoad->getName() );
			if( p ) {
				for( set<string>::iterator it = p->begin(); it != p->end(); it++ ){
					pvt->push_back( *it );
				}
			}
			pvt->push_back( vLoad->getName() );
			//printAlias( vLoad->getName() );
			//DBG;
		} else if( isa<SExtInst>(v) ) {
			SExtInst* EI = cast<SExtInst>(v);
			
			Value* va = EI->getOperand(0);
			errs() << *va << "\n";
			delete pvt;
			pvt = GetAllAliasVars(va);			
			
			print_vector_str(pvt);
			
			return pvt;
		} else {
			set<string>* p = GetAlias( v->getName() );
			if( p ) {
				for( set<string>::iterator it = p->begin(); it != p->end(); it++ ){
					pvt->push_back( *it );
				}
			}
			//printAlias( v->getName() );
		}
		return pvt;
	}
	
	// helper functions 
	void print_vector_str(vector<string>* pvt)
	{
		errs() << "print_vector_str: size = " << pvt->size() << " -------------\n";
		for( unsigned int i = 0; i < pvt->size(); i++ ){
			errs() << " [" << i << "]: " << (*pvt)[i] << "\n";
		}
		errs() << "----------------------------------------\n";
	}
	
	void AnalyzeDecodeFunction(Function* F)
	{
		string str;
		llvm::raw_string_ostream rso(str);
		
		F->getReturnType()->print(rso);
		string s = rso.str();
		//errs() << s << "\n\n\n";
		if( s == string("i8*") ) {
			errs() << "Function Returns char*\n";
		}
		
		set<string> setArgs;
		map<string, string> mapMem;
		
		for (auto& A : F->getArgumentList()) {
			Value* v = A.stripPointerCasts();
			errs() << "[A] " << v->getName() << "\n";
			setArgs.insert(v->getName().str());
		}
		
		set<string> set_retVal;
		for(Function::iterator BB = F->begin(); BB != F->end(); BB++){
			for(BasicBlock::iterator I = BB->begin(); I != BB->end(); I++){
				if( isa<ReturnInst>(I) ) {
					Instruction* retInst = I;
					if( retInst->getNumOperands() > 0 ) {
						Value* retVal = retInst->getOperand(0)->stripPointerCasts();
						//errs() << retVal->getName() << "\n";
						set_retVal.insert( retVal->getName().str() );
					}
				} else if( isa<StoreInst>(I) ) {
					Value* vSrc = I->getOperand(0)->stripPointerCasts();
					Value* vDest = I->getOperand(1)->stripPointerCasts();
					if( vSrc->hasName() ) {
						//errs() << "[S]" << vSrc->getName() << "=>" << vDest->getName() << "\n";
						//mapMem[vDest->getName().str()] = vSrc->getName().str(); 
						NewAlias( vDest->getName().str(), vSrc->getName().str() );
						//errs() << "----" << *I << "\n";
					}
				} else if( isa<LoadInst>(I) ) {
					Value* vMem = I->getOperand(0)->stripPointerCasts();
					string s = mapFind( mapMem, vMem->getName().str() );
					if( s != string("") ) { // found
						//errs() << "[L]" << *I << "---------- " << org->second << "\n";
					} else {
						//errs() << "[L]" << *I << "\n";
					}
				}
				
			}
		}
		
		for(Function::iterator BB = F->begin(); BB != F->end(); BB++){
			for(BasicBlock::iterator I = BB->begin(); I != BB->end(); I++){
				bool bUseReturn = false;
				vector<Value*> vtOps;
				for( unsigned int i = 0; i < I->getNumOperands(); i++ ){
					Value* v = I->getOperand(i)->stripPointerCasts();
					
					vtOps.push_back( v );
					if( set_retVal.find( v->getName().str() ) != set_retVal.end() ) {
						bUseReturn = true;
					}
				}
				if( bUseReturn ) {
					if( GetFunctionCallTarget(I) == string("strcpy") ) {
						Value* vDst = vtOps[0];
						Value* vSrc = vtOps[1];
						//errs() << "------------\n";
						//errs() << *vSrc << "\n";						
						vector<string>* pvt = GetAllAliasVars( vSrc );						
						
						for( unsigned int i = 0; i < pvt->size(); i++ ) {
							NewAlias( vDst->getName().str(), (*pvt)[i] );
						}
						//print_vector_str(pvt);
						
						delete pvt;
						//printAlias( vSrc->getName().str() );
						//string s = mapLookup( mapMem, vSrc->getName().str() );
						//errs() << "------------\n";
					} else if ( isa<GetElementPtrInst>(I) ) {
						GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(I);
						errs() << *GEP << "\n";
						errs() << "ptroperand: " << *(GEP->getPointerOperand()) << "\n";
						
						
						if( GEP->getNumOperands() == 3 ) {
							Value* vZero = GEP->getOperand(1);
							Value* vIdx = GEP->getOperand(2);
							if ( isa<Constant>(vZero) ) {
								Constant* cv = dyn_cast<Constant>(vZero);
								if( cv->isZeroValue() ) {								
									errs() << *(vIdx) << "\n";
									GetAllAliasVars(vIdx);
								}
							}
						}
						
						
						//errs() << "bUseReturn:" << *I << "\n";
					}
				}
				//errs() << *I << " ----------- ";
			//	errs() << I->getOpcodeName() << "\n";					
			}
		}
	}
	
	bool runOnModule(Module& M)  
	{
		GetGlobalVariableList( M );		
		
		for(Module::iterator F = M.begin(); F != M.end(); F++){
			if( F->getName().str() == string("decode") ) {
				AnalyzeDecodeFunction(F);
			}
				
			for(Function::iterator BB = F->begin(); BB != F->end(); BB++){
				for(BasicBlock::iterator I = BB->begin(); I != BB->end(); I++){
					//
					// We filter function calls for the "decode" function. 
					//
					if( GetFunctionCallTarget(I) == string("decode") ) {
						//errs() << I->getNumOperands() << "\n";
						
						//
						// for a function call, the first operand is the argument
						// and the second operand is the function call target.
						//
						// stripPointerCasts: If you do not strip the pointer cast, 
						// the LLVM considers the variable with a pointer cast is new variable. 
						// It assigns a new variable ID. 
						// For instance, "var" (value ID=1) != "(type*) var" (value ID=2).
						// With the stripPointerCasts, it will be,
						// "var" (value ID=1) != "var" (value ID=1).
						// Essentially, stripPointerCasts will remove all the casting.
						//
						// ==========================================================
						// Strip off pointer casts, all-zero GEPs, address space casts.
						// ==========================================================
						// 
						// Refer: https://llvm.org/doxygen/classllvm_1_1Value.html#a966eb231e7d4e572874d2cb49b18faea
						//
						Value* arg = I->getOperand(0)->stripPointerCasts(); 
						if( IsGlobalVariable( arg ) ) {
							errs() << "Global Variable:" << arg->getName() << "\n";
							//runOnDecodeFunction(f);
						}
					}
					//errs() << *I << "\n";
				}
			}
		}
		return false;
	}
  };
}

char MyLLVMPass::ID = 0;
static RegisterPass<MyLLVMPass> X("MyLLVMPass", "MyLLVMPass");

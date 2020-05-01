1. Listing All Global Variables.
>> M.getGlobalList is the key data structure.
```c++
void ListGlobalVariable(Module &M) {
for(auto G=M.getGlobalList().begin();G!=M.getGlobalList().end();G++){
GlobalVariable &GV=*G;
errs() << " GV: " << GV << "\n";
}
}
```

2. Identfiy whether a global variable holds a constant.
>> isConstant is the key method.
```c++
{
GlobalVariable &GV=*G;
if( GV.isConstant() ) { ... }
}
```

3. Get a (constant) value from a global variable
>> getInitializer is the key method
```c++
{
GlobalVariable &GV=*G;
Constant *const_gv = GV.getInitializer();
...
}
```

4. Get a constant string from the Constant*
>> Casting it to ConstantDataArray and calling getAsCString function are key.
```c++
{
Constant *const_gv = ...;
auto data_array = dyn_cast<ConstantDataArray>(const_gv);
if (data_array) {
std::string str_value = data_array->getAsCString();
// str_value holds the string value of const_gv.
}
}
```

5. Replace a value of a global variable.
>> setInitializer is the key method.
```c++
{
// M = Module
// szData = char[]
Constant *const_array = ConstantDataArray::getString(M.getContext(), szData, true);
GV.setInitializer(const_array);
}
```

6. Getting and Setting an argument of a call instruction.
```c++
{
// I = Instruction
CallSite cs(&I); // cast it into CallSite
if(CallInst* call_inst = dyn_cast<CallInst>(&I)) { // Cast it into CallInst
// Getting an argument
Value* arg = cs.getArgument(n); // get the (n+1)th argument
// Setting an argument
cs.setArgument(n, newval); // set the (n+1)th argument with newval (type: Value*).
}
}
```

7. Create a function call.
```c++
{
//
// M = Module
// I = Current Inst.
//
Function* func = M.getFunction(/* function name as a string*/);
errs() << "func: " << func << "\n";

Instruction* nextInst = I.getNextNode();
// LoadInst and CallInst are created and inserted before the specified instruction passed to the functions. The reason that it gets nextInst is to insert the new instruction before the next instruction and after the current instruction.

LoadInst* ptr_load = new LoadInst(pvTo, "", false, nextInst);
ptr_load->setAlignment(8);

CallInst* ptr_call= CallInst::Create(func, ptr_load, "call", nextInst);
ptr_call->setCallingConv(CallingConv::C);
ptr_call->setTailCall(false);
}
```

8. Remove an instruction
```c++
{
Instruction* i_to_delete = 0;
...
if( i_to_delete ) i_to_delete->eraseFromParent();
}
```
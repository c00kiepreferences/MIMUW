package Instructions.Singular.Declarations;

import Expressions.Exp;
import Expressions.Variable;
import Processor.Memory;
import Processor.ProgramExe;

public class Declaration_V extends Declaration {
    private final Variable var;
    private final Exp exp;

    public Declaration_V(Variable var, Exp exp) {
        super();
        this.var = var;
        this.exp = exp;
        var.setScope(scope);
    }

    @Override
    public void setScope(int scope) {
        this.scope = scope;
        var.setScope(this.scope);
    }

    public Variable getVar() {
        return var;
    }

    @Override
    public void run() {
        preRun();
        if (var.getName() > 'z' || var.getName() < 'a') ProgramExe.runtime_error();
        Memory.declareV(var, exp.count());
        ProgramExe.makeStep();
    }

    @Override
    public String toString() {
        return "declare " + var.getName() + " = " + exp.toString();
    }
}

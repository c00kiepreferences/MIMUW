package Instructions.Singular;

import Expressions.Exp;
import Expressions.Variable;
import Processor.*;

public class Assign extends Singular {
    private final Variable var;
    private final Exp exp;

    public Assign(Variable var, Exp exp) {
        super();
        this.var = var;
        this.exp = exp;
    }

    @Override
    public void setScope(int scope) {
        this.scope = scope;
        var.setScope(this.scope);
    }

    @Override
    public void run() {
        preRun();
        Memory.setValue(var, exp.count());
        ProgramExe.makeStep();
    }

    @Override
    public String toString() {
        return var.toString() + " = " + exp.toString();
    }
}

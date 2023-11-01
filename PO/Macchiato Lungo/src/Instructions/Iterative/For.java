package Instructions.Iterative;

import Expressions.Exp;
import Expressions.Variable;
import Instructions.Instruction;
import Instructions.ProcedureCall;
import Instructions.Singular.Assign;
import Instructions.Singular.Print;
import Processor.Memory;

import java.util.LinkedList;
import java.util.List;

public class For extends Iterative {
    private final Variable var;
    private final Exp exp;
    private final List<Instruction> instructions;

    public For(Variable var, Exp exp, List<Instruction> instructions) {
        super();
        this.var = var;
        this.exp = exp;
        this.instructions = instructions;
    }

    public For(ForBuilder builder) {
        super();
        var = builder.var;
        exp = builder.exp;
        instructions = builder.instructions;
    }

    @Override
    public void setScope(int scope) {
        this.scope = scope + 1;
        var.setScope(this.scope);
        for (Instruction i : instructions) i.setScope(this.scope);
    }

    @Override
    public void run() {
        preRun();
        int laps = exp.count();
        var.setScope(this.scope);
        Memory.declareV(var, 0);
        for (int i = 0; i < laps; i++){
            Memory.setValue(var, i);
            for (Instruction j : instructions) j.run();
        }

        Memory.deleteV(var);
    }

    @Override
    public String toString() {
        String ret = "for (";
        ret += var.toString() + " < ";
        ret += exp.toString() + ") {\n";
        for (Instruction i : instructions) {
            ret += i.toString() + "\n";
        }
        ret += "}";
        return ret;
    }

    public static class ForBuilder {
        private List<Instruction> instructions;
        private final Variable var;
        private final Exp exp;

        public ForBuilder (char name, Exp exp) {
            this.var = new Variable(name);
            this.exp = exp;
            instructions = new LinkedList<>();
        }

        public ForBuilder assign (char name, Exp exp) {
            instructions.add(new Assign(new Variable(name), exp));
            return this;
        }

        public ForBuilder print (Exp exp) {
            instructions.add(new Print(exp));
            return this;
        }

        public ForBuilder rep (For foo) {
            instructions.add(foo);
            return this;
        }

        public ForBuilder iff (If foo) {
            instructions.add(foo);
            return this;
        }

        public ForBuilder block (Block foo) {
            instructions.add(foo);
            return this;
        }

        public ForBuilder call(String name, List<Exp> values) {
            instructions.add(new ProcedureCall(name, values));
            return this;
        }

        public For build() {
            return new For(this);
        }
    }

}
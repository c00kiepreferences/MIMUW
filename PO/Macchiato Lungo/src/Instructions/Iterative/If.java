package Instructions.Iterative;

import Conditions.Condition;
import Expressions.Exp;
import Expressions.Variable;
import Instructions.Instruction;
import Instructions.ProcedureCall;
import Instructions.Singular.Assign;
import Instructions.Singular.Print;

import java.util.LinkedList;
import java.util.List;

public class If extends Iterative {
    private final Condition condition;
    private boolean checked = false;
    private final List<Instruction> if_true;
    private final List<Instruction> if_false;
    private List<Instruction> instructions;

    public If(Condition condition, List<Instruction> if_true, List<Instruction> if_false) {
        super();
        this.condition = condition;
        this.if_true = if_true;
        this.if_false = if_false;
    }

    public If(IfBuilder builder) {
        super();
        condition = builder.condition;
        if_true = builder.if_true;
        if_false = builder.if_false;
    }

    public If(Condition condition, List<Instruction> if_true){
        this(condition, if_true, List.of());
    }

    private void check() {
        if (checked) return;
        checked = true;
        if (!condition.outcome()) instructions = if_false;
        else instructions = if_true;
    }

    @Override
    public void setScope(int scope) {
        this.scope = scope;
        for (Instruction i : if_true) i.setScope(this.scope);
        for (Instruction i : if_false) i.setScope(this.scope);
    }

    @Override
    public void run() {

        preRun();

        check();
        for (Instruction i : instructions) i.run();
        checked = false;
    }

    @Override
    public String toString() {
        String ret = "if(" + condition.toString() + ") {\n";
        ret += "then:\n";
        for (Instruction i : if_true) {
            ret += i.toString() + "\n";
        }
        if (if_false.size() != 0) {
            ret += "else:\n";
            for (Instruction i : if_false) {
                ret += i.toString() + "\n";
            }
        }
        ret += "}";
        return ret;
    }

    public static class IfBuilder {
        private List<Instruction> if_true;
        private List<Instruction> if_false;
        private List<Instruction> to_add;
        private final Condition condition;

        public IfBuilder (Condition con) {
            this.condition = con;
            if_true = new LinkedList<>();
            if_false = new LinkedList<>();
            to_add = if_true;
        }

        public IfBuilder assign (char name, Exp exp) {
            to_add.add(new Assign(new Variable(name), exp));
            return this;
        }

        public IfBuilder print (Exp exp) {
            to_add.add(new Print(exp));
            return this;
        }

        public IfBuilder rep (For foo) {
            to_add.add(foo);
            return this;
        }

        public IfBuilder iff (If foo) {
            to_add.add(foo);
            return this;
        }

        public IfBuilder block (Block foo) {
            to_add.add(foo);
            return this;
        }

        public IfBuilder or_else () {
            to_add = if_false;
            return this;
        }

        public IfBuilder call(String name, List<Exp> values) {
            to_add.add(new ProcedureCall(name, values));
            return this;
        }

        public If build() {
            return new If(this);
        }
    }
}

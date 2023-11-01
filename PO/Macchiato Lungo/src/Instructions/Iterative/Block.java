package Instructions.Iterative;

import Expressions.Exp;
import Expressions.Variable;
import Instructions.Instruction;
import Instructions.ProcedureCall;
import Instructions.Singular.Assign;
import Instructions.Singular.Declarations.*;
import Instructions.Singular.Print;
import Processor.Memory;
import Processor.ProgramExe;

import java.beans.Expression;
import java.util.*;

public class Block extends Iterative {
    private List<Instruction> instructions;
    private List<Declaration> initialization;

    public Block(List<Declaration> initialization, List<Instruction> instructions) {
        super();
        this.initialization = initialization;
        this.instructions = instructions;
    }

    public Block(BlockBuilder builder) {
        super();
        initialization = builder.initialization;
        instructions = builder.instructions;
    }

    @Override
    public void setScope(int scope) {
        this.scope = scope + 1;
        for (Declaration i : initialization) i.setScope(this.scope);
        for (Instruction i : instructions) i.setScope(this.scope);
    }

    @Override
    public void run() {

        preRun();

        for (Declaration i : initialization) i.run();
        for (Instruction i : instructions) i.run();

        if (scope == 1) {
            System.out.print("Program finished.\n");
            Memory.display(0);
        } else for (Declaration i : initialization) {
            if (i instanceof Declaration_V)
                Memory.deleteV(((Declaration_V) i).getVar());
            if (i instanceof Declaration_P)
                Memory.deleteP(((Declaration_P) i).getName());
        }
    }

    @Override
    public String toString() {
        String ret = "---Block---\n";
        ret += "---Block declarations:---\n";
        for (Declaration i : initialization) {
            ret += i.toString() + "\n";
        }
        ret += "---Block instructions:---\n";
        for (Instruction i : instructions) {
            ret += i.toString() + "\n";
        }
        ret += "---End of block---";
        return ret;
    }

    public static class BlockBuilder {
        private List<Instruction> instructions;
        private List<Declaration> initialization;

        public BlockBuilder() {
            instructions = new LinkedList<>();
            initialization = new LinkedList<>();
        }

        public BlockBuilder declareVariable(char name, Exp exp) {
            initialization.add(new Declaration_V(new Variable(name), exp));
            return this;
        }

        public BlockBuilder declareProcedure(String name, List<Character> arguments, Block contents) {
            initialization.add(new Declaration_P(name, arguments, contents));
            return this;
        }

        public BlockBuilder assign(char name, Exp exp) {
            instructions.add(new Assign(new Variable(name), exp));
            return this;
        }

        public BlockBuilder print(Exp exp) {
            instructions.add(new Print(exp));
            return this;
        }

        public BlockBuilder rep(For foo) {
            instructions.add(foo);
            return this;
        }

        public BlockBuilder iff(If foo) {
            instructions.add(foo);
            return this;
        }

        public BlockBuilder block(Block foo) {
            instructions.add(foo);
            return this;
        }

        public BlockBuilder call(String name, List<Exp> values) {
            instructions.add(new ProcedureCall(name, values));
            return this;
        }

        public Block build() {
            return new Block(this);
        }
    }
}

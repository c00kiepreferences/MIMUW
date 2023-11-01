package Instructions;

import Expressions.Exp;
import Expressions.Variable;
import Instructions.Iterative.Block;
import Processor.Memory;
import Processor.ProgramExe;

import java.util.List;

public class ProcedureCall extends Instruction {

    private final String name;
    private final List<Exp> values;
    private List<Character> variables;
    private Block contents;

    public ProcedureCall(String name, List<Exp> values) {
        super();
        this.name = name;
        this.values = values;
    }

    @Override
    public void setScope(int scope) {
        super.setScope(scope + 1);
    }

    private void prepare() {
        variables = Memory.getVariables(name);
        contents = Memory.getContents(name);
        if (variables.size() != values.size()) ProgramExe.runtime_error();
        for (int i = 0; i < variables.size(); i++) {
            Variable var = new Variable(variables.get(i));
            var.setScope(scope);
            Memory.declareV(var, values.get(i).count());
        }
    }

    @Override
    public void run() {
        preRun();
        prepare();
        contents.setScope(scope);
        contents.run();
        for (char i : variables) Memory.deleteV(new Variable(i));
    }

    @Override
    public String toString() {
        return null;
    }
}

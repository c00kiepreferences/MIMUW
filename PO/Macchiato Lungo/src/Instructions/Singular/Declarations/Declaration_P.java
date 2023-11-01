package Instructions.Singular.Declarations;

import Instructions.Iterative.Block;
import Processor.Memory;
import Processor.ProgramExe;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Declaration_P extends Declaration {

    private final String name;
    private final List<Character> arguments;
    private final Block contents;

    public Declaration_P(String name, List<Character> arguments, Block contents) {
        super();
        this.name = name;
        this.arguments = arguments;
        this.contents = contents;
    }

    public String getName() {
        return name;
    }

    @Override
    public void setScope(int scope) {
        this.scope = scope;
        contents.setScope(this.scope);
    }

    @Override
    public void run() {
        preRun();
        Set<Character> vars = new HashSet<>();
        for (char i : arguments) {
            if (i < 'a' || i > 'z') ProgramExe.runtime_error();
            if (vars.contains(i)) ProgramExe.runtime_error();
            vars.add(i);
        }
        Memory.declareP(name, arguments, contents, scope);
        ProgramExe.makeStep();
    }

    @Override
    public String toString() {
        String ret = "declare " + name + " ( ";
        for (char i : arguments) ret += i + ", ";
        ret += ")";
        return ret;
    }
}

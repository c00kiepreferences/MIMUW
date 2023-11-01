package Instructions.Singular;

import Expressions.Exp;
import Processor.*;

public class Print extends Singular {
    private final Exp exp;

    public Print(Exp exp) {
        super();
        this.exp = exp;
    }

    @Override
    public void run() {
        preRun();
        System.out.print(exp.count());
        System.out.print('\n');
        ProgramExe.makeStep();
    }

    @Override
    public String toString() {
        return "print " + exp.toString();
    }
}

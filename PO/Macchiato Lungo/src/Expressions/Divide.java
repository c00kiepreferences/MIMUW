package Expressions;

import Processor.ProgramExe;

public class Divide extends TwoArgument {

    public Divide(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        sign = "/";
    }

    public static Divide of(Exp a, Exp b) {
        return new Divide(a, b);
    }

    @Override
    public int count() {
        if (exp2.count() == 0) ProgramExe.runtime_error();
        return exp1.count() / exp2.count();
    }
}

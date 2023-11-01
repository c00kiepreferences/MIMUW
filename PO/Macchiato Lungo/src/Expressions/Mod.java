package Expressions;

import Processor.ProgramExe;

public class Mod extends TwoArgument {

    public Mod(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        sign = "mod";
    }

    public static Mod of(Exp a, Exp b) {
        return new Mod(a, b);
    }

    @Override
    public int count() {
        if (exp2.count() == 0) ProgramExe.runtime_error();
        return exp1.count() % exp2.count();
    }
}

package Expressions;

import java.beans.Expression;

public class Minus extends TwoArgument {

    public Minus(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        sign = "-";
    }

    public static Minus of(Exp a, Exp b) {
        return new Minus(a, b);
    }

    @Override
    public int count() {
        return exp1.count() - exp2.count();
    }
}

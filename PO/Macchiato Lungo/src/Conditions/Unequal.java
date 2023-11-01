package Conditions;

import Expressions.Exp;
import Expressions.Minus;

public class Unequal extends Condition {
    public Unequal(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        symbol = "<>";
    }

    public static Unequal of(Exp a, Exp b) {
        return new Unequal(a, b);
    }

    public boolean outcome() {
        return exp1.count() != exp2.count();
    }
}

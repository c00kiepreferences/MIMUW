package Conditions;

import Expressions.Exp;
import Expressions.Minus;

public class Less extends Condition {
    public Less(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        symbol = "<ł";
    }

    public static Less of(Exp a, Exp b) {
        return new Less(a, b);
    }

    public boolean outcome() {
        return exp1.count() < exp2.count();
    }
}

package Conditions;

import Expressions.Exp;
import Expressions.Minus;

public class Meq extends Condition {
    public Meq(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        symbol = ">=";
    }

    public static Meq of(Exp a, Exp b) {
        return new Meq(a, b);
    }

    public boolean outcome() {
        return exp1.count() >= exp2.count();
    }
}

package Conditions;

import Expressions.Exp;
import Expressions.Minus;

public class Equals extends Condition {
    public Equals(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        symbol = "=";
    }

    public static Equals of(Exp a, Exp b) {
        return new Equals(a, b);
    }

    public boolean outcome() {
        return exp1.count() == exp2.count();
    }
}

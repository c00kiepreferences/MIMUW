package Conditions;

import Expressions.Exp;
import Expressions.Minus;

public class More extends Condition {
    public More(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        symbol = ">";
    }

    public static More of(Exp a, Exp b) {
        return new More(a, b);
    }

    public boolean outcome() {
        return exp1.count() > exp2.count();
    }
}

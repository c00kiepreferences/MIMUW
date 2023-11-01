package Expressions;

public class Multiply extends TwoArgument {

    public Multiply(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        sign = "*";
    }

    public static Multiply of(Exp a, Exp b) {
        return new Multiply(a, b);
    }

    @Override
    public int count() {
        return exp1.count() * exp2.count();
    }
}

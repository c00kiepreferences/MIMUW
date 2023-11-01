package Expressions;

public class Plus extends TwoArgument {

    public Plus(Exp exp1, Exp exp2) {
        super(exp1, exp2);
        sign = "+";
    }

    public static Plus of(Exp a, Exp b) {
        return new Plus(a, b);
    }

    @Override
    public int count() {
        return exp1.count() + exp2.count();
    }
}

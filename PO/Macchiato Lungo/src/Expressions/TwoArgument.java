package Expressions;

public abstract class TwoArgument extends Exp {

    protected Exp exp1, exp2;
    protected String sign;

    protected TwoArgument(Exp exp1, Exp exp2) {
        super();
        this.exp1 = exp1;
        this.exp2 = exp2;
    }

    @Override
    public String toString() {
        return "(" + exp1.toString() + " " + sign + " " + exp2.toString() + ")";
    }
}

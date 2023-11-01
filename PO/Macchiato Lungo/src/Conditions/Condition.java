package Conditions;

import Commands.Command;
import Expressions.Exp;

public abstract class Condition extends Command {
    protected String symbol;
    protected Exp exp1, exp2;

    public abstract boolean outcome();

    public Condition(Exp exp1, Exp exp2) {
        super();
        this.exp1 = exp1;
        this.exp2 = exp2;
    }

    @Override
    public String toString() {
        return exp1.toString() + " " + symbol + " " + exp2.toString();
    }

}

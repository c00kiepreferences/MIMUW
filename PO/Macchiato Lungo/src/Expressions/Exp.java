package Expressions;

import Commands.Command;
import Instructions.Instruction;

public abstract class Exp extends Command {

    protected Exp() {
        super();
    }

    public abstract int count();

}

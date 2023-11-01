package Commands;

import Instructions.Instruction;
import Processor.Memory;

public abstract class Command {

    protected int scope;

    protected Command() {
        scope = 0;
    }

    public void setScope(int scope) {
        this.scope = scope;
    }

    public int getScope() {
        return scope;
    }

    public abstract String toString();
}

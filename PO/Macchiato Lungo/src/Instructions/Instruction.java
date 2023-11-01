package Instructions;

import Commands.Command;
import Expressions.Exp;
import Processor.ProgramExe;

public abstract class Instruction extends Command {

    protected Instruction() {
        super();
    }

    protected void preRun (){
        ProgramExe.setNowExecuted(this);
        ProgramExe.ask();
    }

    public abstract void run();
}

package Instructions.Singular;

import Expressions.Literal;
import Expressions.Variable;
import Instructions.Iterative.Block;
import Instructions.Singular.Declarations.Declaration;
import Instructions.Singular.Declarations.Declaration_V;
import Processor.Memory;
import Processor.ProgramExe;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AssignTest {

    @Test
    void setScope() {
        Variable a = Variable.named('a');
        Assign ass = new Assign(a, Literal.of(0));
        ass.setScope(1);
        assertEquals(ass.getScope(), 1);
        assertEquals(a.getScope(), 1);
    }

    @Test
    void run() {
        Declaration_V dec = new Declaration_V(Variable.named('a'), Literal.of(1));
        dec.run();
        Assign ass = new Assign(Variable.named('a'), Literal.of(2));
        ass.run();
        assertEquals(Memory.getValue(Variable.named('a')), 2);
    }
}
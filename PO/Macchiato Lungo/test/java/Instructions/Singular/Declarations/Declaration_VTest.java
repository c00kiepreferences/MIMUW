package Instructions.Singular.Declarations;

import Expressions.Literal;
import Expressions.Variable;
import Processor.Memory;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class Declaration_VTest {

    @Test
    void getVar() {
        Variable var = Variable.named('a');
        Declaration_V dec = new Declaration_V(var, Literal.of(0));
        assertEquals(dec.getVar(), var);
    }

    @Test
    void run() {
        Variable var = Variable.named('a');
        Declaration_V dec = new Declaration_V(var, Literal.of(0));
        dec.run();
        assertEquals(Memory.getValue(var), 0);
    }
}
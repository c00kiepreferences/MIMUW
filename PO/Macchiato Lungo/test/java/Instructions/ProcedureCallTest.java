package Instructions;

import Expressions.Literal;
import Expressions.Variable;
import Instructions.Iterative.Block;
import Instructions.Singular.Assign;
import Instructions.Singular.Declarations.Declaration_P;
import Processor.Memory;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class ProcedureCallTest {

    @Test
    void setScope() {
        ProcedureCall prc = new ProcedureCall("foo", List.of(Literal.of('a')));
        prc.setScope(0);
        assertEquals(prc.getScope(), 1);
    }

    @Test
    void run() {
        Block contents = new Block.BlockBuilder()
                .declareVariable('a', Literal.of(1))
                .build();
        Declaration_P dec = new Declaration_P("foo", List.of(), contents);
        dec.run();
        ProcedureCall prc = new ProcedureCall("foo", List.of());
        prc.run();
        assertEquals(Memory.getValue(Variable.named('a')), 1);

    }
}
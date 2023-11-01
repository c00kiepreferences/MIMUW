package Instructions.Iterative;

import Conditions.Equals;
import Conditions.Unequal;
import Expressions.Literal;
import Expressions.Variable;
import Instructions.Instruction;
import Instructions.Singular.Declarations.Declaration_V;
import Instructions.Singular.Print;
import Processor.Memory;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class IfTest {

    @Test
    void setScope() {
        List<Instruction> if_true = List.of(new Print(Literal.of(1)));
        List<Instruction> if_false = List.of(new Print(Literal.of(1)));
        Literal z = Literal.of(0);
        If iff = new If (Equals.of(z, z), if_true, if_false);
        iff.setScope(0);
        assertEquals(if_true.get(0).getScope(), 0);
        assertEquals(if_false.get(0).getScope(), 0);
    }

    @Test
    void run() {
        Declaration_V dec = new Declaration_V(Variable.named('a'), Literal.of(2));
        dec.run();
        Literal z = Literal.of(0);

        If if1 = new If.IfBuilder(Equals.of(z, z))
                .assign('a', Literal.of(1))
                .or_else()
                .assign('a', Literal.of(0))
                .build();
        if1.run();
        assertEquals(Memory.getValue(Variable.named('a')), 1);

        If if2 = new If.IfBuilder(Unequal.of(z, z))
                .assign('a', Literal.of(1))
                .or_else()
                .assign('a', Literal.of(0))
                .build();
        if2.run();
        assertEquals(Memory.getValue(Variable.named('a')), 0);
    }
}
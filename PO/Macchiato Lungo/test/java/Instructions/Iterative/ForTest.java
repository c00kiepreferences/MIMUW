package Instructions.Iterative;

import Expressions.Literal;
import Expressions.Variable;
import Instructions.Instruction;
import Instructions.Singular.Assign;
import Instructions.Singular.Declarations.Declaration_V;
import Instructions.Singular.Print;
import Processor.Memory;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class ForTest {

    @Test
    void setScope() {
        List<Instruction> contents = List.of(new Print(Literal.of(1)));
        For loop = new For(Variable.named('a'), Literal.of(0), contents);
        loop.setScope(0);
        assertEquals(contents.get(0).getScope(), 1);
    }

    @Test
    void run() {
        Declaration_V deca = new Declaration_V(Variable.named('a'), Literal.of(0));
        Declaration_V decb = new Declaration_V(Variable.named('b'), Literal.of(0));
        For loop = new For.ForBuilder('b', Literal.of(10))
                .assign('a', Variable.named('b'))
                .assign('b', Literal.of(400))
                .build();
        deca.run();
        decb.run();
        loop.setScope(1);
        loop.run();
        assertEquals(Memory.getValue(Variable.named('a')), 9);
        assertEquals(Memory.getValue(Variable.named('b')), 0);
    }
}
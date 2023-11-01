package Instructions.Iterative;

import Expressions.Literal;
import Expressions.Variable;
import Instructions.Instruction;
import Instructions.Singular.Declarations.Declaration_V;
import Processor.Memory;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class BlockTest {

    private ByteArrayOutputStream catcher;

    @Test
    void setScope() {
        List<Instruction> contents = List.of(new Declaration_V(Variable.named('a'), Literal.of(1)));
        Block blo = new Block(List.of(), contents);
        blo.setScope(0);
        assertEquals(blo.getScope(), 1);
        assertEquals(contents.get(0).getScope(), 1);
    }

    @Test
    void run() {
        catcher = new ByteArrayOutputStream();
        System.setOut(new PrintStream(catcher));
        Block blo = new Block.BlockBuilder()
                .declareVariable('a', Literal.of(1))
                .build();
        blo.setScope(0);
        blo.run();
        assertEquals(catcher.toString().trim(), "Program finished.\na = 1");

        catcher = new ByteArrayOutputStream();
        System.setOut(new PrintStream(catcher));
        blo = new Block.BlockBuilder()
                .declareVariable('a', Literal.of(1))
                .build();
        blo.setScope(1);
        blo.run();
        assertEquals(catcher.toString().trim(), "");
    }
}
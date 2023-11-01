package Instructions.Singular.Declarations;

import Instructions.Iterative.Block;
import Processor.Memory;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class Declaration_PTest {

    @Test
    void getName() {
        Declaration_P dec = new Declaration_P("foo", List.of(), new Block.BlockBuilder().build());
        assertEquals(dec.getName(), "foo");
    }

    @Test
    void setScope() {
        Block blo = new Block.BlockBuilder().build();
        Declaration_P dec = new Declaration_P("foo", List.of(), blo);
        dec.setScope(0);
        assertEquals(dec.getScope(), 0);
        assertEquals(blo.getScope(), 1);
    }

    @Test
    void run() {
        Block contents = new Block.BlockBuilder().build();
        List<Character> arguments = List.of('a');
        Declaration_P dec = new Declaration_P("foo", arguments, contents);
        dec.run();
        assertEquals(Memory.getContents("foo"), contents);
        assertEquals(Memory.getVariables("foo"), arguments);
    }
}
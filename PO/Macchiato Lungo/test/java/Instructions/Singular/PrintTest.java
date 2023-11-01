package Instructions.Singular;

import Expressions.Literal;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import static org.junit.jupiter.api.Assertions.*;

class PrintTest {

    private final ByteArrayOutputStream catcher = new ByteArrayOutputStream();

    @Test
    void run() {
        System.setOut(new PrintStream(catcher));
        Print pri = new Print(Literal.of(10));
        pri.run();
        assertEquals(catcher.toString().trim(), "10");
    }
}
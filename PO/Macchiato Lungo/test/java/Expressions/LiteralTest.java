package Expressions;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class LiteralTest {

    @Test
    void count() {
        Literal x = Literal.of(1);
        assertEquals(x.count(), 1);
    }
}
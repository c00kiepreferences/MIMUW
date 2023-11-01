package Expressions;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MinusTest {

    @Test
    void count() {
        Minus x = Minus.of(Literal.of(10), Literal.of(5));
        assertEquals(x.count(), 5);
    }
}
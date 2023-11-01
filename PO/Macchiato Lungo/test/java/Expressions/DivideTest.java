package Expressions;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class DivideTest {

    @Test
    void count() {
        Divide x = Divide.of(Literal.of(10), Literal.of(5));
        assertEquals(x.count(), 2);
    }
}
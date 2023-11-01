package Expressions;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MultiplyTest {

    @Test
    void count() {
        Multiply x = Multiply.of(Literal.of(10), Literal.of(5));
        assertEquals(x.count(), 50);
    }
}
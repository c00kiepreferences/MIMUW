package Expressions;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class PlusTest {

    @Test
    void count() {
        Plus x = Plus.of(Literal.of(10), Literal.of(5));
        assertEquals(x.count(), 15);
    }
}
package Expressions;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class ModTest {

    @Test
    void count() {
        Mod x = Mod.of(Literal.of(10), Literal.of(3));
        assertEquals(x.count(), 1);
    }
}
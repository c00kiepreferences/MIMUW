package Conditions;

import Expressions.Literal;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class UnequalTest {

    @Test
    void outcome() {
        Unequal x = Unequal.of(Literal.of(1), Literal.of(1));
        assert(!x.outcome());
    }
}
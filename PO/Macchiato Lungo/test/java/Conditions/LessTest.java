package Conditions;

import Expressions.Literal;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class LessTest {

    @Test
    void outcome() {
        Less x = Less.of(Literal.of(1), Literal.of(1));
        assert(!x.outcome());
    }
}
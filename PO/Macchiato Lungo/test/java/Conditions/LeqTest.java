package Conditions;

import Expressions.Literal;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class LeqTest {

    @Test
    void outcome() {
        Leq x = Leq.of(Literal.of(1), Literal.of(2));
        assert(x.outcome());
    }
}
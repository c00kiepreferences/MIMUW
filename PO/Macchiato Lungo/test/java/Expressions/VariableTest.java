package Expressions;

import Processor.Memory;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class VariableTest {

    @Test
    void getName() {
        Variable x = Variable.named('x');
        assertEquals(x.getName(), 'x');
    }

    @Test
    void count() {
        Memory.declareV(Variable.named('x'), 1);
        Variable x = Variable.named('x');
        assertEquals(Memory.getValue(Variable.named('x')), 1);
    }
}
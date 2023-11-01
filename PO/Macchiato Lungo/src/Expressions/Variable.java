package Expressions;

import Processor.Memory;

public class Variable extends Exp {
    private final Character name;

    public Variable(Character name) {
        super();
        this.name = name;
    }

    public Character getName() {
        return name;
    }

    @Override
    public int count() {
        return Memory.getValue(this);
    }

    public static Variable named(char name) {
        return new Variable(name);
    }

    @Override
    public String toString() {
        return Character.toString(name);
    }
}

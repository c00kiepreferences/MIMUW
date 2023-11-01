package Expressions;

public class Literal extends Exp {
    private final int value;

    public Literal(int value) {
        super();
        this.value = value;
    }

    public static Literal of(int a) {
        return new Literal(a);
    }

    @Override
    public int count() {
        return value;
    }

    @Override
    public String toString() {
        return Integer.toString(value);
    }
}

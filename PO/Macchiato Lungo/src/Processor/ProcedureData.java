package Processor;

import Expressions.Variable;
import Instructions.Iterative.Block;

import java.util.List;

public class ProcedureData {
    public String name;
    public List<Character> variables;
    public Block block;

    public int scope;

    public ProcedureData(String name, List<Character> variables, Block block, int scope) {
        this.name = name;
        this.variables = variables;
        this.block = block;
        this.scope = scope;
    }
}

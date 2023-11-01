package Processor;

import Expressions.Variable;

import java.io.*;
import java.util.*;

import Instructions.Iterative.Block;

public class Memory {
    private static final Map<Character, Vector<VarData>> variables = new HashMap<>();
    private static final Map<String, Vector<ProcedureData>> procedures = new HashMap<>();

    public static void declareV(Variable var, int value) {
        if (!variables.containsKey(var.getName()))
            variables.put(var.getName(), new Stack<>());
        if (!variables.get(var.getName()).isEmpty()) {
            if (variables.get(var.getName()).lastElement().scope == var.getScope())
                ProgramExe.runtime_error();
        }
        variables.get(var.getName()).add(new VarData(value, var.getScope()));
    }

    public static void declareP(String name, List<Character> arguments, Block contents, int scope) {
        if (!procedures.containsKey(name))
            procedures.put(name, new Stack<>());
        if (!procedures.get(name).isEmpty()) {
            if (procedures.get(name).lastElement().scope == scope)
                ProgramExe.runtime_error();
        }
        procedures.get(name).add(new ProcedureData(name, arguments, contents, scope));
    }

    public static void deleteV(Variable var) {
        Vector stack = variables.get(var.getName());
        stack.remove(stack.size() - 1);
        if (variables.get(var.getName()).isEmpty()) variables.remove(var.getName());
    }

    public static void deleteP(String name) {
        Vector stack = procedures.get(name);
        stack.remove(stack.size() - 1);
        if (procedures.get(name).isEmpty()) procedures.remove(name);
    }

    public static void setValue(Variable var, int value) {
        Vector stack = variables.get(var.getName());
        if (stack.isEmpty()) ProgramExe.runtime_error();
        stack.remove(stack.size() - 1);
        stack.add(new VarData(value, var.getScope()));
    }

    public static int getValue(Variable var) {
        if (!variables.containsKey(var.getName())) ProgramExe.runtime_error();
        return variables.get(var.getName()).lastElement().value;
    }

    public static List<Character> getVariables(String name) {
        if (!procedures.containsKey(name)) ProgramExe.runtime_error();
        return procedures.get(name).lastElement().variables;
    }

    public static Block getContents(String name) {
        if (!procedures.containsKey(name)) ProgramExe.runtime_error();
        return procedures.get(name).lastElement().block;
    }

    public static void display(int floor) {

        for (Map.Entry<Character, Vector<VarData>> entry : variables.entrySet()) {
            for (VarData var : entry.getValue()) {
                if (var.scope <= ProgramExe.getActScope() - floor) {
                    System.out.print(entry.getKey() + " = " + var.value + '\n');
                    break;
                }
            }
        }
        for (Map.Entry<String, Vector<ProcedureData>> entry : procedures.entrySet()) {
            for (ProcedureData procedure : entry.getValue()) {
                if (procedure.scope <= ProgramExe.getActScope() - floor) {
                    System.out.print(pToString(procedure));
                    break;
                }
            }
        }
    }

    public static void dump(String path) {
        try {
            File file = new File(path);
            FileWriter writer = new FileWriter(file);
            for (Map.Entry<Character, Vector<VarData>> entry : variables.entrySet()) {
                writer.write(entry.getKey() + " = " + entry.getValue().lastElement().value + '\n');
            }

            System.out.print('\n');
            for (Map.Entry<String, Vector<ProcedureData>> entry : procedures.entrySet()) {
                writer.write(pToString(entry.getValue().lastElement()));
            }
            writer.close();
        } catch (IOException e) {
            System.err.print("Memory dump fail.\n");
        }
        System.out.print("dumped succesfully\n");
    }

    private static String pToString(ProcedureData procedure) {
        String ret = procedure.name + " (";
        for (char i : procedure.variables) ret += i + ", ";
        ret = ret.substring(0, ret.length() - 2);
        ret += ")\n";
        return ret;
    }
}

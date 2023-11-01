package Processor;

import Instructions.Instruction;
import Instructions.Iterative.Block;

import java.io.IOException;
import java.util.Scanner;

public class ProgramExe {
    private static Block main;
    private static Instruction nowExecuted;

    private static int currScope;
    private static int steps;
    private static boolean debug = false, finished = false;
    private static Scanner scanner = new Scanner(System.in);

    public static void ask() {
        if (!debug) return;
        if (steps > 0) return;
        if (currScope != 0) System.out.print(nowExecuted.toString() + '\n');
        terminal();
    }

    public static void setNowExecuted(Instruction instr){
        if (nowExecuted != null) currScope = nowExecuted.getScope();
        nowExecuted = instr;
    }

    public static void setMain(Block block) {main = block;}

    public static int getActScope() {
        return nowExecuted.getScope();
    }

    public static void makeStep() {
        if (!debug) return;
        steps--;
    }

    public static void run() {
        main.setScope(0);
        currScope = 0;
        main.run();
        finished = true;
    }

    public static void debug() {
        steps = 0;
        debug = true;
        run();
        terminal();
    }

    public static void terminal() {
        while (true){
            switch (scanner.next().charAt(0)) {
                case 'e' -> System.exit(0);
                case 'c' -> {
                    if (finished) {
                        System.out.print("Program already Finished.\n");
                        break;
                    }
                    debug = false;
                    return;
                }
                case 'd' -> Memory.display(scanner.nextInt());
                case 's' -> {
                    steps = scanner.nextInt();
                    if (finished) {
                        System.out.print("Program already Finished.\n");
                        break;
                    }
                    if (steps < 0) {
                        System.out.print("No rewinding time!\n");
                        break;
                    }
                    if (steps == 0) break;
                    return;
                }
                case 'm' -> Memory.dump(scanner.next());
                default -> System.out.print("There is no such command :<\n");
            }
        }
    }

    public static void runtime_error () {
        System.err.print("Instruction impossible to execute:\n");
        System.err.print(nowExecuted + "\n");
        System.err.print("Variables visible in this scope:\n");
        Memory.display(0);
        System.exit(0);
    }

}

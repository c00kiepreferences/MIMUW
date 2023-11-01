import Conditions.*;
import Expressions.*;
import Instructions.*;
import Instructions.Singular.*;
import Instructions.Singular.Declarations.*;
import Instructions.Iterative.*;
import Processor.ProgramExe;

import java.util.List;

public class Main {
    public static void main(String[] args) {
        test2(false);
    }

    //test z pierwszego na moodle,
    //program wypisuje liczby pierwsze mniejsze od 30
    /*---------------------------------------------------------
    begin block
        var n 30
        for k n-1
            begin block
            var p 1
            k := k+2
            for i k-2
                i := i+2
                if k % i = 0
                p := 0
            if p = 1
                print k
            end block
    end block
    *///-------------------------------------------------------
    /* private static void test1(boolean czy_debug) {
        Exp pom;

        Exp exp_czy1 = new Mod(new Variable('k'), new Variable('i'));
        Condition con_czy1 = new Equals(exp_czy1, new Literal(0));
        Instruction[] instr_czy1 = {new Assign(new Variable('p'), new Literal(0))};
        If czy1 = new If(con_czy1, instr_czy1);

        Condition con_czy2 = new Equals(new Variable('p'), new Literal(1));
        Instruction[] instr_czy2 = {new Print(new Variable('k'))};
        If czy2 = new If(con_czy2, instr_czy2);

        Exp exp_for2 = new Minus(new Variable('k'), new Literal(2));
        pom = new Plus(new Variable('i'), new Literal(2));
        Instruction[] instr_for2 = {new Assign(new Variable('i'), pom), czy1};
        For for2 = new For(new Variable('i'), exp_for2, instr_for2);

        Declaration_V[] ini_blok2 = {new Declaration_V(new Variable('p'), new Literal(1))};
        pom = new Plus(new Variable('k'), new Literal(2));
        Instruction[] instr_blok2 = {new Assign(new Variable('k'), pom), for2, czy2};
        Block blok2 = new Block(ini_blok2, instr_blok2);

        Exp exp_for1 = new Minus(new Variable('n'), new Literal(1));
        Instruction[] instr_for1 = {blok2};
        For for1 = new For(new Variable('k'), exp_for1, instr_for1);

        Declaration_V[] ini_blok1 = {new Declaration_V(new Variable('n'), new Literal(30))};
        Instruction[] instr_blok1 = {for1};
        Block blok1 = new Block(ini_blok1, instr_blok1);

        //System.out.print(blok1.to_String() + '\n');
        ProgramExe.setMain(blok1);
        if (czy_debug) ProgramExe.debug();
        else ProgramExe.run();
    } */

    //test z drugiego zadania na moodle
    /*---------------------------------------------------------
    begin block
        var x 101
        var y 1
        proc out(a)
            print a+x
        end proc
        x := x - y
        out(x)
        out(100)  // tu powinno wypisać 200
        begin block
            var x 10
            out(100) // tu statycznie wciąż 200, dynamicznie 110
        end block
    end block
    *///-------------------------------------------------------
    private static void test2(boolean czy_debug) {

        List<Declaration> block2_init = List.of(new Declaration_V(new Variable('x'), new Literal(10)));
        List<Exp> call = List.of(new Literal(100));
        List<Instruction> block2_instr = List.of(new ProcedureCall("out", call));
        Block block2 = new Block(block2_init, block2_instr);

        List<Declaration> blockF_init = List.of();
        List<Instruction> blockF_instr = List.of(new Print(new Plus(new Variable('a'), new Variable('x'))));
        Block contents = new Block(blockF_init, blockF_instr);
        Declaration_P out = new Declaration_P("out", List.of('a'), contents);

        List<Exp> call2 = List.of(new Variable('x'));
        List<Declaration> main_init = List.of(
                new Declaration_V(new Variable('x'), new Literal(101)),
                new Declaration_V(new Variable('y'), new Literal(1)), out
        );
        List<Instruction> main_instr = List.of(
                new Assign(new Variable('x'), new Minus(new Variable('x'), new Variable('y'))),
                new ProcedureCall("out", call2),
                new ProcedureCall("out", call),
                block2
        );
        Block main = new Block(main_init, main_instr);

        //System.out.print(main.to_String() + '\n');
        ProgramExe.setMain(main);
        if (czy_debug) ProgramExe.debug();
        else ProgramExe.run();
    }

    /*---------------------------------------------------------
    begin block
        var x 101
        var y 1
        proc out(a)
            print a+x
        end proc
        x := x - y
        out(x)
        out(100)  // tu powinno wypisać 200
        begin block
            var x 10
            out(100) // tu statycznie wciąż 200, dynamicznie 110
        end block
    end block
    *///-------------------------------------------------------
    public static void test2_builder (boolean czy_debug) {
        ProgramExe.setMain(
                new Block.BlockBuilder()
                        .declareVariable('x', Literal.of(101))
                        .declareVariable('y', Literal.of(1))
                        .declareProcedure("out", List.of('a'), new Block.BlockBuilder()
                                .print(Plus.of(Variable.named('a'), Variable.named('x')))
                                .build()
                        )
                        .assign('x', Minus.of(Variable.named('x'), Variable.named('y')))
                        .call("out", List.of(Variable.named('x')))
                        .call("out", List.of(Literal.of(100)))
                        .block(new Block.BlockBuilder()
                                .declareVariable('x', Literal.of('x'))
                                .call("out", List.of(Literal.of(100)))
                                .build()
                        )
                        .build()
        );
        if (czy_debug) ProgramExe.debug();
        else ProgramExe.run();
    }

}
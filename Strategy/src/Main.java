public class Main {
    public static void main(String[] args) {
        Context context = new Context();

        context.setStrategy(new ConcreteStrategyAdd());
        int resultA = context.executeStrategy(3,4);

        context.setStrategy(new ConcreteStrategySubtract());
        int resultB = context.executeStrategy(3,4);

        context.setStrategy(new ConcreteStrategyMultiply());
        int resultC = context.executeStrategy(3,4);

        System.out.println("Значение A: " + resultA );
        System.out.println("Значение B: " + resultB );
        System.out.println("Значение C: " + resultC );
    }
}

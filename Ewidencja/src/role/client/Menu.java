package role.client;

import role.client.command.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Map;

public class Menu {
    private Map<Integer, Command> _menuCommands;

    public Menu(Map<Integer, Command> commands) {
        this._menuCommands = commands;
    }

    public void draw() {
        for (Map.Entry<Integer, Command> entry: this._menuCommands.entrySet()) {
            System.out.println(entry.getKey() + ". " + entry.getValue().name());
        }
        System.out.println("\nWybierz numer opcji z menu: ");
    }

    public Command readChoice() {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        while (true) {
            try {
                String input = bufferedReader.readLine();
                Integer option_no = Integer.parseInt(input);
                Command selected = this._menuCommands.get(option_no);
                if (selected == null) {
                    throw new NullPointerException();
                }
                return selected;
            } catch (IOException e) {
                System.out.println("Błąd operacji wejścia-wyjścia");
            } catch (NullPointerException e) {
                System.out.println("Podaj poprawny numer opcji: ");
            }
        }
    }
}

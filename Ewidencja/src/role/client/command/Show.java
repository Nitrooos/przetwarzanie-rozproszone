package role.client.command;

import data.Database;
import data.Person;
import role.client.Menu;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Map;
import java.util.TreeMap;

public class Show implements Command {

    private Menu _editMenu;

    @Override
    public boolean run(Database db) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        System.out.println("Podaj ID osoby, o której chcesz wyświetlić informacje: ");
        try {
            String input = bufferedReader.readLine();
            Integer id = Integer.parseInt(input);
            Person selected = db.get(id);
            if (selected != null) {
                System.out.println("Imię i nazwisko: " + selected.getFullName());
                System.out.println("Data urodzenia: " + new SimpleDateFormat("dd.MM.yyyy").format(selected.getInfo().getBirth()));
                selected.printMarriagesInfo();
                selected.printChildrenInfo();
                selected.printParentInfo();

                this.initSubmenu(id, selected);
                while (true) {
                    this._editMenu.draw();
                    Command c = this._editMenu.readChoice();
                    if (!c.run(db)) {
                        break;
                    }
                }
            } else {
                System.out.println("Nie znaleziono osoby o ID " + id + " w bazie danych\n");
            }
        } catch (Exception e) {
            System.out.println("Błąd we-wy: " + e.getMessage());
        }

        return true;
    }

    @Override
    public String name() {
        return "Pokaż szczegóły osoby";
    }

    private void initSubmenu(Integer id, Person p) {
        Map<Integer, Command> commands = new TreeMap<Integer, Command>() {{
            put(3, new Quit("do menu głównego"));
        }};

        if (Calendar.getInstance().getTime().getYear() - p.getInfo().getBirth().getYear() > 18) {
            commands.put(1, new Marriage(id, p));
        }

        if (p.isMarried()) {
            commands.put(2, new AddChild(p.getCurrentMarriage().get()));
        }

        this._editMenu = new Menu(commands);
    }
}

package role.client.command;

import data.*;
import data.Marriage;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Date;

public class AddChild implements Command {

    private Marriage _marriage;

    public AddChild(data.Marriage marriage) {
        this._marriage = marriage;
    }

    @Override
    public boolean run(Database db) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        try {
            System.out.println("Podaj imię dziecka: ");
            String first_name = bufferedReader.readLine();

            System.out.println("Podaj płeć dziecka (M lub K): ");
            String gender_input = bufferedReader.readLine();
            BaseInfo.Gender gender = BaseInfo.Gender.parse(gender_input);

            BaseInfo child_info = new BaseInfo(first_name, this._marriage.getHusband().getInfo().getLastname(), new Date(), gender);
            Person child = this._marriage.haveChild(child_info);
            db.create(child);

            Integer h_id = db.findId(this._marriage.getHusband()),
                    w_id = db.findId(this._marriage.getWife());
            db.update(h_id, this._marriage.getHusband());
            db.update(w_id, this._marriage.getWife());

            System.out.println("Dodano nową osobę do bazy");
        } catch (IOException e) {
            System.out.println("Błąd obsługi wejścia-wyjścia: " + e.getMessage());
        } catch (RuntimeException e) {
            System.out.println("Błędne dane: " + e.getMessage());
        }

        return true;
    }

    @Override
    public String name() {
        return "Dodaj dziecko";
    }
}

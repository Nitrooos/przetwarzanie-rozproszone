package data.seed;

import data.BaseInfo;

import java.util.*;

public class BaseInfoGenerator {
    static Map<BaseInfo.Gender, List<String>> _firstnames = new TreeMap<BaseInfo.Gender, List<String>>(){{
        put(BaseInfo.Gender.MALE, new ArrayList<String>() {{
            add("Jan");
            add("Stanisław");
            add("Andrzej");
            add("Tadeusz");
            add("Jerzy");
            add("Krzysztof");
            add("Henryk");
            add("Ryszard");
            add("Marek");
            add("Adam");
        }});
        put(BaseInfo.Gender.FEMALE, new ArrayList<String>() {{
            add("Maria");
            add("Krystyna");
            add("Anna");
            add("Barbara");
            add("Teresa");
            add("Elżbieta");
            add("Zofia");
            add("Danuta");
            add("Ewa");
            add("Małgorzata");
        }});
    }};

    static List<String> _lastnames = new ArrayList<String>() {{
        add("Nowak");
        add("Kowalski");
        add("Wiśniewski");
        add("Dąbrowski");
        add("Lewandowski");
        add("Wójcik");
        add("Kamiński");
        add("Miauczyński");
        add("Zieliński");
        add("Kozłowski");
    }};

    static public BaseInfo generate(BaseInfo.Gender gender, int age) {
        Random r = new Random();
        Date d = new Date(Calendar.getInstance().get(Calendar.YEAR) - age - 1900, r.nextInt(12) + 1, r.nextInt(28) + 1);
        List<String> proper_firstnames = BaseInfoGenerator._firstnames.get(gender);
        String random_firstname = proper_firstnames.get(r.nextInt(proper_firstnames.size()));
        String random_lastname  = BaseInfoGenerator._lastnames.get(r.nextInt(BaseInfoGenerator._lastnames.size()));

        return new BaseInfo(random_firstname, random_lastname, d, gender);
    }
}

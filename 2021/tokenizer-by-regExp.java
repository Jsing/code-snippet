import java.util.regex.Matcher;
import java.util.regex.Pattern;

void tokenizeByRegExp() {
    String inputText = "A101B4C22";
    Pattern pattern = Pattern.compile("[A-Z][0-9]+");
    Matcher matcher = pattern.matcher(inputText);

    while(matcher.find()) {
        System.out.println(matcher.group());
    }
}
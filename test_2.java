/**
 * Test comment, the good and the bad.
 */

import com.s.b.Something;

public class A {
	static public void main() {// some comment.
		boolean c// TTTTT 
		;
		int a = Something.get(12/* Some comment */, 13);
		String s ="this is a string, haha /***/";
		return;
	}
}

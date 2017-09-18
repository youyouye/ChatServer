package com.muduo.chat;

public abstract class TestCase {
	
	protected MultiplexerTest god;
	protected EventQueue queue;
	
	public void setOwner(MultiplexerTest god){
		this.god = god;
		queue = god.getEventQueue();
	}	
	
	public void test(){
		try {
			run();
		} catch (TestFailedException e) {
			System.out.printf("%s FAILED: %s\n",this.getClass().getSimpleName(),e.getMessage());
			e.printStackTrace();
			return;
		}catch (Exception e) {
			System.out.printf("%s FATAL: %s\n",this.getClass().getSimpleName(), e.toString());
			e.printStackTrace();
			return;
		}
	}
	
	protected void assertEquals(Object expected, Object actual) {
        if (!expected.equals(actual))
            fail("assertEquals failed");
    }

    protected void assertTrue(boolean yes) {
        if (!yes)
            fail("assertTrue failed");
    }

    protected void fail(String message) {
        throw new TestFailedException(message);
    }
	
	public abstract void run();
}

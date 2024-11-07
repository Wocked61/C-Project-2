// Name: Dylan Phan
// CWID: 885582536
// Email: DylanP@csu.fullerton.edu

/**
 * TODO: Implement all the BookStore methods below.
 * Note you'll have to remain inside the CPSC131::BookStore namespace.
 */

//
#include "BookStore.hpp"


//
#include <iostream>
#include <sstream>
#include <string>


//
using std::cout, std::cin, std::endl;
using std::string, std::to_string;
using std::stringstream;


//
namespace CPSC131::BookStore
{

	/// Your welcome
	BookStore::BookStore() {}
	
	/**
	 * Adjust the store's account balance
	 * Should accept positive or negative adjustments
	 */
	void BookStore::adjustAccountBalance(int adjustment)
	{
		//	TODO: Your code here
		account_balance_ += adjustment;
	}
	
	/**
	 * Return the store's current account balance
	 */
	int BookStore::getAccountBalance() const
	{
		//	TODO: Your code here
		
		return account_balance_;
	}
	
	/**
	 * Find a book by its ISBN
	 * 
	 * Return this->bookList.end() if the book isn't found.
	 * 
	 * Return an interator pointing to the Book if it is found.
	 */
	DoublyLinkedList::DoublyLinkedList<Book>::Iterator BookStore::findBook(std::string isbn) const
	{
		//	TODO: Your code here
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it = bookList.begin();
		while (it.getCursor() != it.end())
		{
			DoublyLinkedList::DoublyLinkedList<Book>::Node* temp = it.getCursor();
			if (temp->getElement().getIsbn() == isbn) {
				return it;
			}
			it++;
		}
		return this->bookList.end();
	}
	
	/**
	 * Check whether a book exists, by its ISBN
	 * 
	 * Return true if it exists, or false otherwise
	 */
	bool BookStore::bookExists(std::string isbn) const
	{
		//	TODO: Your code here
		if (findBook(isbn) != bookList.end()) {
			return false;
		}
		return true;
	}
	
	/**
	 * Check the quantity of stock we have for a particular book, by ISBN
	 * 
	 * If the book doesn't exist, just return 0
	 */
	size_t BookStore::getBookStockAvailable(std::string isbn) const
	{
		//	TODO: Your code here
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it = findBook(isbn);
		if (it != bookList.end()) {
			return it.getCursor()->getElement().getStockAvailable();
		} else {
		return 0;	
		}
	}
	
	/**
	 * Locate a book by ISBN and return a reference to the Book
	 * 
	 * If the book doesn't exist, throw an exception
	 */
	Book& BookStore::getBook(std::string isbn) const
	{
		//	TODO: Your code here
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it = findBook(isbn);
		if (it != bookList.end()) {
			return it.getCursor()->getElement();
		}
		throw std::range_error("book not found");
	}
	
	/**
	 * Take a Book instance and add it to inventory
	 * 
	 * If the book's ISBN already exists in our store,
	 * 	simply adjust account balance by the book's price and quantity,
	 * 	but ignore other details like title and author.
	 * 
	 * If the book's ISBN doesn't already exist in our store,
	 * 	adjust our account balance and push the book into our list
	 */
	void BookStore::purchaseInventory(const Book& book)
	{
		//	TODO: Your code here
		Book temp = book;
		if (bookExists(temp.getIsbn())) {
			getBook(temp.getIsbn()).adjustStockAvailable(temp.getStockAvailable());
		} else {
			bookList.push_back(book);
		}
		adjustAccountBalance(-1*(temp.getPriceCents()*temp.getStockAvailable()));

	}
	
	/**
	 * Take some book details and add the book to our inventory.
	 * 
	 * Use the same rules as the other overload for this function.
	 * 
	 * You might want to avoid repeating code by simply building a Book
	 * 	object from the details, then calling the other overload
	 * 	with the new Book object.
	 */
	void BookStore::purchaseInventory(
		std::string title, std::string author, std::string isbn,
		size_t price_cents,
		size_t unit_count
	)
	{
		//	TODO: Your code here
		Book book = Book(title, author, isbn, price_cents, unit_count);
		purchaseInventory(book);
	}
	
	/**
	 * Print out inventory.
	 * Should be in a particular way to earn unit test points.
	 * Example (ignore the asterisks at the left of this comment block):
	 * 
	 * *** Book Store Inventory ***
	 * "Book1", by Author1 [123] (5 in stock)
	 * "Book2", by Author2 [456] (19 in stock)
	 * 
	 * Should print a trailing std::endl after the last book line, for the unit tests
	 */
	void BookStore::printInventory() const
	{
		//	TODO: Your code here
		std::cout << "*** Book Store Inventory ***" << endl;
		for (const Book& book : bookList) {
			std::cout << "\"" << book.getTitle() << "\"" << ", by " << book.getAuthor()
			<< " [" << book.getIsbn() << "]" << " (" << book.getStockAvailable()
			<< " in stock)" << endl;
		}
	}
	
	/**
	 * Sell a book to a customer!
	 * 
	 * Takes the ISBN of the book, the selling price of the book, and the quantity of books sold
	 * 
	 * Uses the same rules as the other overload.
	 * 
	 * You may wish to just grab a reference to the book and call the other overload,
	 * 	to avoid repeating code
	 */
	void BookStore::sellToCustomer(std::string isbn, size_t price_cents, size_t quantity)
	{
		//	TODO: Your code here
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it = findBook(isbn);
		if (it != bookList.end()) {
			if (quantity > it.getCursor()->getElement().getStockAvailable()) {
				throw std::range_error("Quantity of books unavaliable");
			}
			if (it.getCursor()->getElement().getStockAvailable() == 0) {
				throw std::range_error("Book is out of stock");
			}
			adjustAccountBalance(price_cents*quantity);
			it.getCursor()->getElement().adjustStockAvailable(-1*(quantity));
		}
	}
	
	/**
	 * Sell a book to a customer!
	 * 
	 * Takes a Book reference, the selling price of the book, and the quantity of books sold
	 * 
	 * If we don't have enough of this book in stock for the quantity the customer wants to purchase,
	 * 	throw an std::range_error
	 * 
	 * Otherwise, adjust the stock available in our store, and update our account balance.
	 */
	void BookStore::sellToCustomer(Book& book, size_t price_cents, size_t quantity)
	{
		//	TODO: Your code here
		sellToCustomer(book.getIsbn(), price_cents, quantity);
	}

}








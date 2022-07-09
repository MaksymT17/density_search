/**
 *
 * matrix class to store data from image.
 *
 * @author Maksym V
 */


#include <vector>
#include <iostream>

class Matrix {
public:
	Matrix(const size_t width, const size_t height)
		: mWidth(width), mHeight(height) {
		try {
			mData = std::vector<std::vector<int>>(height, std::vector<int>(width));
		}
		catch (const std::bad_alloc& e) {
			std::cout << "Allocation of the Matrix data failed: " << e.what() << std::endl;
		}
		catch (...) {
			std::cout << "Allocation failed" << std::endl;
		}
	}

	Matrix() = delete;

	virtual ~Matrix() = default;

	size_t getWidth() const noexcept { return mWidth; }
	size_t getHeight() const noexcept { return mHeight; }

	int& operator()(size_t i, size_t j)
	{
		return  mData[i][j];
	}

	int operator()(size_t i, size_t j) const
	{
		return  mData[i][j];
	}

private:
	const size_t mWidth;
	const size_t mHeight;

	std::vector<std::vector<int>> mData;
};
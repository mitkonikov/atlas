class Graham:
    def __init__(self) -> None:
        pass

    def solve(self, points: List[Point]) -> List[Point]:
        n = len(points)
        average = Point(0, 0)
        for point in points:
            average = average.add(point)

        average = average.div(Point(n, n))
        
        points.sort(key = lambda point: point.sub(average).angle())

        left = 0
        for i in range(n):
            if points[i].x < points[left].x:
                left = i

        stack = []
        for i in range(n + 1):
            id = (i + left) % n
            while (len(stack) >= 2 and stack[-2].triangle(stack[-1], points[id]) < 0):
                stack.pop()
            stack.append(points[id])

        return stack